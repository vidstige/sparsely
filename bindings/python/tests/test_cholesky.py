import numpy as np

from sparsely import cholesky, solve_cholesky

from .matrices import easy2x2


def test_factorization():
    A = easy2x2().tocsr()
    L = cholesky(A)

    expected = np.array([
        [2.0, 0.0],
        [1.0, np.sqrt(2.0)]
    ])
    assert np.allclose(L.todense(), expected, rtol=1e-6, atol=1e-12)


def test_solve_2x2():
    A = easy2x2().tocsr()
    b = np.array([8.0, 8.0])
    L = cholesky(A)  # find factorization
    L.sort_indices()  # sort indices to ensure diagonal is last
    x = solve_cholesky(L, b)  # solve

    # Expected solution
    expected = np.array([1.0, 2.0])

    np.testing.assert_allclose(x, expected, rtol=1e-6, atol=1e-12)
